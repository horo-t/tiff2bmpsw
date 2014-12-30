mergeInto(LibraryManager.library, {
    writeBmp: function(buf, size) {
        bmpOutput.push(Module.HEAPU8.buffer.slice(buf, buf + size));
    }
});
