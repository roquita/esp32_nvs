# NON VOLATILE STORAGE
## Create NVS image
Use NVS partition tool to generate binary image. This may be accordingly partitions.csv.
## Flash image:
```python
python C:/esp41/esp-idf/components/partition_table/parttool.py --port COM18 write_partition --partition-name=nvs --input "nvs.bin"
```
## Read image:
```python
python C:/esp41/esp-idf/components/partition_table/parttool.py read_partition  --partition-name=nvs --output "flashed.bin"
```
## Use data from NVS in "C" code:
Using namespace and keys to read and write data. Check main.c code.
