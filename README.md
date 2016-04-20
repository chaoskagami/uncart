# uncart
3DS homebrew utility program to dump game cartridges to the SD card.

This fork is a9lh-compatible, based on the patch released by Plailect. It contains only the minimal changes for a9lh, not the fancy (albeit likely proper) resize code.

This branch contains several UI improvements:
- Progress bar at bottom to measure precise read progress
- No eye-murder colors (in favor of standard white-on-black)
- Rewinding for repeated messages to avoid readability issues arising from looping text
- Less interrobangs

Credits go to:
- archshift: code cleanups; general polishing of my PoC dumper
- Lioncash: code cleanups
- neobrain: getting the project started; on-the-fly decryption
- Normmatt: doing tons of reverse-engineering work; providing the core dumping code
- yuriks: compatibility enhancements
- d0k3: reboot patch
- Gelex: The original a9lh port
