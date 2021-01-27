# ReduceMyMap
A MPI-based map-reduce program designed to take in some documents and to output frequency of words in every document.

## Prerequisities

 * Sometin' able to compile C++17
 * Visual Studio may help but it's not necessary
 * [Microsoft MPI][5] if you're on Windows or [Open MPI][6] if you're on Linux
 * [nlohmann/json][2]

 ## Usage

This should be obvious, but build the application and run the application you just built.
The program should receive 2 arguments:
 - `-i /path/to/input` to tell the program where to read files from
 - `-o /path/to/output` to tell the program where to write files to

A temporary directory will also be created, but you should live with the idea that you can't change that easily.

The program will loop through all files in the directory, read them and apply the mapping to each one.

## License

 * This thing is distributed under Apache 2.0. See [LICENSE](LICENSE).
 * This thing uses [nlohmann/json][2] which is distributed under [MIT License][4]
 * Gitignore file from [github/gitignore][3].

## Additional details

### Contact

You can find me [here][1] to ask questions.

[1]: https://github.com/Vasile2k
[2]: https://github.com/nlohmann/json
[3]: https://github.com/github/gitignore
[4]: https://github.com/nlohmann/json/blob/develop/LICENSE.MIT
[5]: https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi
[6]: https://www.open-mpi.org/