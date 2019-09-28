# LoL-Language-Changer

## Building it yourself
If you want to compile the project yourself, compile the intercepter first, then replace loc_selection.exe in the resources with yours and check that its SHA256 matches the one set in the GUI source code (macro TOOL_HASH in widget.h). If it doesn't, simply replace it with yours, otherwise your build won't be recognized.
(Qt is needed to build the GUI app)
