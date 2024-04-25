# 42-pipex
> Redirections and pipes.

Pipex is a program that replicates the behavior of doing `< infile cmd1 | cmd2 > outfile` in bash, or even `< infile cmd1 | ... | cmdn > outfile`.

The program will receive al least 4 arguments where the first one will be the input file from it will read, and the last one will be the output file that will contain the final outputs.

```shell
./pipex infile cmd1 cmd2 cmd3 outfile
```

See the [wiki](https://github.com/Aryalexa/42-pipex/wiki) for documentation.

## Usage

Clone the project
```sh
git clone --recursive git@github.com:Aryalexa/42-pipex.git
```
Compile the files and generate the `pipex` executable.
```sh
make
```
Examples of usage:
```
./pipex infile "ls -l" "wc -l" outfile
./pipex infile "grep a1" "wc -w" outfile
```

