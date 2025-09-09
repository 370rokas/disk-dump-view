# disk-dump-view
 .raw disk dump explorer

Learning project where I seek to learn more about partitions, filesystems, etc..

TODO:
-[x] Identify partition tables.
-[ ] Parse partition table data.
-[ ] ID partitions.
-[ ] Read partition filesystems.
-[ ] Extract files
-[ ] ..? (coming soon)

## Testing

```bash
# Create the test disk's
sudo dnf install dosfstools
sudo bash scripts/createTestDrives.sh
```