"use strict"

run("v8-function-call")
run("node-make-callback")

function run(name) {
    const {spawnSync} = require("node:child_process")
    const args = [`./bench/${name}.js`]
    spawnSync(process.execPath, args, {stdio: "inherit"})
}
