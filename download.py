import requests
import os

base_url = "https://placedata.reddit.com/data/canvas-history/2022_place_canvas_history-{:012d}.csv.gzip"

download_count = 79

data_folder = "data/"

for i in range(download_count):
    srcurl = base_url.format(i)
    zipdestpath = data_folder + "{:012d}.csv.gz".format(i)

    print("Downloading from", srcurl, "to", zipdestpath)
    request = requests.get(srcurl)
    open(zipdestpath, 'wb').write(request.content)

    os.system("gzip -d " + zipdestpath)