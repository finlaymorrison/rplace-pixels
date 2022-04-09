import requests
import gzip
import shutil
import os

base_url = "https://placedata.reddit.com/data/canvas-history/2022_place_canvas_history-{:012d}.csv.gzip"

download_count = 79

data_folder = "data/"

for i in range(download_count):
    srcurl = base_url.format(i)
    zipdestpath = data_folder + "{:012d}.csv.gz".format(i)
    csvdestpath = data_folder + "{:012d}.csv".format(i)

    print("Downloading from", srcurl, "to", zipdestpath)
    request = requests.get(srcurl)
    open(zipdestpath, 'wb').write(request.content)

    print("Downloading from", zipdestpath, "to", csvdestpath)
    with gzip.open(zipdestpath, 'rb') as f_in:
        with open(csvdestpath, 'wb') as f_out:
            shutil.copyfileobj(f_in, f_out)

    os.remove(zipdestpath)