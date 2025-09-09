# disk-dump-view
 .raw disk dump explorer

Learning project where I seek to learn more about partitions, filesystems, etc..

## Testing

```bash
# Create the test disk's
sudo dnf install dosfstools
sudo bash scripts/createTestDrives.sh
```

## TODO:
- [x] Identify partition tables.
- [x] Parse partition table data.
- [ ] Recognise filesystems.
- [ ] Read partition filesystems.
- [ ] Extract files?
- [ ] ..? (coming soon)
