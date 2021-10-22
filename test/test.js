let segfaultHandler = require('../index');

function foo() {
    bar();
}

function bar() {
    segfaultHandler.segfault();
}

function main() {
    segfaultHandler.registerHandler();
    if (process.env['TEST_SEGFAULT']) {
        foo();
    }
}

main();