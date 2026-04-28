# ANS - Alphabetical Numeric System

ANS is a custom positional numeral system based on alphabetic characters instead of digits.
Each letter represents a numeric value ($A = 0, B = 1, \dots, Z = 25$), similar to how hexadefical or other base-N systems work.

This simple program allows these conversions:

* decimal number to ANS string
* ANS string to decimal number

## Building

1. Clone this repository and open a terminal in the cloned repo
2. Run `make` command. The compiled `ans.exe` will be created in the repo's root folder:
   ```bash
   make
   ```

## Usage

`./ans [options]`

options:

* `-h` or `--help` → displays help
* `-i` or `--info` → shows detailed program description
* `-r` or `--raw` → output result only (raw output)
* `-a <number>` or `--to-alpha <number>` → converts `<number>` to `<ANS string>`
* `-n <ANS string>` or `--to-num <ANS string>` → converts `<ANS string>` to a decimal number

### Examples

```bash
$ ./ans -a 27
-a "27" -> BB

$ ./ans -n BC
-n "BC" -> 28

$ ./ans -r -a 53
CB
  
$ ./ans -r -n CD
28
```

## Notes

* **This project was originally created on 7.1.2026; I'm uploading it to GitHub just for archival purposes**
* Input in decimal numbers **must** **be** **non-negative**
* Use shell redirection (`>`) to save output to a file
