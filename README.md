# Advent of Code 2020

## Initialize cmake
```shell
mkdir bin
cmake -Bbin -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES 
```

## Build all targets
```shell
make -C bin
```

## Run the code
```shell
./bin/day1/day1 day1/input.txt
./bin/day2/day2 day2/input.txt
```

## (optional) symlink compile_commands.json for ccls to pick up
```shell
ln -s bin/compile_commands.json
```
