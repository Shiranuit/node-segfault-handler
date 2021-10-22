let segfaultHandler = require('bindings')('SegfaultHandler');

module.exports = {
    registerHandler: segfaultHandler.registerHandler,
    segfault: segfaultHandler.segfault
};