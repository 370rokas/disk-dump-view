import os, subprocess, sys, argparse


def runCmd(cmd: list[str]):
    """Run shell command and print it."""
    print(">>>", " ".join(cmd))
    _ = subprocess.run(cmd, check=True)


def createEmptyDisk(filename: str, size_mb: int):
    """Create an empty file of given size (MB)."""
    with open(filename, "wb") as f:
        _ = f.seek(size_mb * 1024 * 1024 - 1)
        _ = f.write(b"\0")
    print(f"Created empty disk image: {filename} ({size_mb} MB)")


def createPartitionTable(filename: str, label: str):
    """Create partition table (MBR or GPT)."""
    runCmd(["parted", "-s", filename, "mklabel", label])


def createPartitions(filename: str, partitions: list[tuple[int, int, str, str]]):
    """Create partitions in the disk image and format them."""
    # Attach loop device
    losetup_out = subprocess.run(
        ["losetup", "--show", "-f", filename],
        capture_output=True,
        text=True,
        check=True,
    )
    loop_device = losetup_out.stdout.strip()
    print(f"Loop device created: {loop_device}")

    try:
        for i, (start, end, ptype, fs) in enumerate(partitions, 1):
            # Create partition
            runCmd(
                ["parted", "-s", filename, "mkpart", ptype, f"{start}MB", f"{end}MB"]
            )
            print(f"Partition {i}: {ptype} {start}MB → {end}MB")

        # Update kernel partition table
        runCmd(["partprobe", loop_device])

        # Format partitions
        for i, (_, _, _, fs) in enumerate(partitions, 1):
            part_device = (
                f"{loop_device}p{i}"
                if os.path.exists(f"{loop_device}p{i}")
                else f"{loop_device}{i}"
            )
            print(f"Formatting {part_device} as {fs}")
            runCmd(["mkfs", "-t", fs, part_device])
    finally:
        # Detach loop device even if an error occurs
        runCmd(["losetup", "-d", loop_device])
        print("Loop device detached.")


def createPartitionArgs(partition_args: list[str]) -> list[tuple[int, int, str, str]]:
    """Parse partitions passed as CLI args into list of tuples (start, end, type, fs)."""
    partitions: list[tuple[int, int, str, str]] = []

    for arg in partition_args:
        try:
            start, end, ptype, fs = arg.split(":")
            partitions.append((int(start), int(end), ptype, fs))
        except ValueError:
            print(
                f"Invalid partition format: {arg}. Use start:end:type:fs (e.g., 1:30:primary:ext4)"
            )
            sys.exit(1)

    return partitions


def main():
    parser = argparse.ArgumentParser(
        description="Create a virtual disk image with partitions and format them."
    )

    _ = parser.add_argument(
        "-o",
        "--output",
        default="virtual_disk.bin",
        help="Output disk image filename (default: virtual_disk.bin)",
    )
    _ = parser.add_argument(
        "-s", "--size", type=int, required=True, help="Total disk size in MB"
    )
    _ = parser.add_argument(
        "-t",
        "--table",
        choices=["msdos", "gpt"],
        required=True,
        help="Partition table type: msdos (MBR) or gpt (GPT)",
    )
    _ = parser.add_argument(
        "-p",
        "--partition",
        action="append",
        required=True,
        help="Partition definition as start:end:type:fs (e.g., 1:30:primary:ext4). Use multiple -p for multiple partitions.",
    )

    args: argparse.Namespace = parser.parse_args()

    if os.geteuid() != 0:
        print("Run as root (needed for parted and mkfs).")
        sys.exit(1)

    partitions = createPartitionArgs(args.partition)

    createEmptyDisk(args.output, args.size)
    createPartitionTable(args.output, args.table)
    createPartitions(args.output, partitions)

    print(f"Done! Disk image saved as {args.output}")


if __name__ == "__main__":
    main()
