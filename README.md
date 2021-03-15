ash
===
The (barely) adequate shell.

What works:
- Basic line editing
- Running programs in
    - current working directory
    - by full path
    - a pre-defined list of locations
        - `/bin`
        - `/usr/bin`
        - `/usr/local/bin`
- Internal commands cd and exit
- IO redirection (> and <)


What remains:
- Robust parser
- Pipes
- Simple autocompletion
- Configuration and history file (XDG compliant)
- Scripting and control flow
