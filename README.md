# Rinha de compiler - (compiler illegal battle)

This is my attempt of making a interpreter in C for the fictional language called rinha for the [rinha of compiler](https://github.com/aripiprazole/rinha-de-compiler)


## The goal

Following the specs described [here](https://github.com/aripiprazole/rinha-de-compiler/blob/main/SPECS.md)
My goal is making it possible to run the ast provided into a C interpreter

## How to run

- Make sure you have rust and cargo installed
- run ```cargo install rinha```
- add rinha bin to your path
- run an examples, you can create any rinha file if you want
- run ```rinha examples/main.rinha > main.json``` for creating the ast in json
- run ```./rinha main.json```

## Bugs

- [ ] Recursion is not working correctly
- [ ] Functions out of scope not being passed forward

