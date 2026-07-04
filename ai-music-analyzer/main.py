from ytmusicapi import YTMusic


def playlistIdExtractor(url):
    print()
    s = url
    idx = s.index("=")
    sub = s[idx + 1 :]
    return sub


def basic_yt(playlistId, limit):
    yt = YTMusic()
    j = 1

    dic = yt.get_playlist(playlistId=playlistId, limit=limit)["tracks"]
    for i in dic:
        if j > limit:
            break
        print(j, ".", i["title"])
        j += 1


def main():
    print("Hi  world")
    Id = playlistIdExtractor(
        url="https://music.youtube.com/playlist?list=PLDIoUOhQQPlWm_njQtKkNIk5RYSGgzomm"
    )
    basic_yt(Id, 30)


if __name__ == "__main__":
    main()
