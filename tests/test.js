let segfaultHandler = require(`${__dirname}/../index`);

function foo() {
  bar();
}

function bar() {
  segfaultHandler.segfault();
}

function main() {
  segfaultHandler.registerHandler();
  foo();
}

main();