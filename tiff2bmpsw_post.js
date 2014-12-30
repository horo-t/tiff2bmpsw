var filecount = 0;

function getBmp(url) {
  return new Promise(function(resolve, reject) {
    fetch(url)
      .then(function(res) { return res.arrayBuffer(); })
      .then(function(buffer) {
        var filename = "tmp" + (++filecount) + ".tiff";
        FS.createDataFile(
            '/',
            filename,
            new Uint8Array(buffer), true, false);
        bmpOutput = [];
        var ret = Module.ccall('tiff_to_bmp',
                               'number',
                               ['string'],
                               [filename])
        if (ret == 0)
          resolve(new Response(new Blob(bmpOutput, {type: "image/bmp"})));
        else
          reject();
      });
  });
}

self.addEventListener('fetch', function(event) {
  if (event.request.url.indexOf('.tif') == -1)
    return;
  event.respondWith(getBmp(event.request.url));
});
