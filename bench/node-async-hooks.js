"use strict"
const {bench, nodeMakeCallback} = require("./util")
const AH = require("node:async_hooks")

AH.executionAsyncId()
AH.triggerAsyncId()
const hook = AH.createHook({init, before, after, destroy, promiseResolve})
hook.enable()
bench("node::MakeCallback + async_hooks", nodeMakeCallback)

function init(asyncId, type, triggerAsyncId, resource) {
}

function before(asyncId) {
}

function after(asyncId) {
}

function destroy(asyncId) {
}

function promiseResolve(asyncId) {
}
