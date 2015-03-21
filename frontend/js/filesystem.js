/**
 * Created by Langstra on 3/21/2015.
 */
function toArray(list) {
    return Array.prototype.slice.call(list || [], 0);
}

function onInitFs(fs) {

}

function listDirectory() {
    var dirReader = fs.root.createReader();
    var entries = [];

    // Call the reader.readEntries() until no more results are returned.
    var readEntries = function() {
        dirReader.readEntries (function(results) {
            if (!results.length) {
                listResults(entries.sort());
            } else {
                entries = entries.concat(toArray(results));
                readEntries();
            }
        }, errorHandler);
    };

    readEntries(); // Start reading dirs.
}

window.requestFileSystem(window.TEMPORARY, 1024*1024, onInitFs, errorHandler);