## Convert and Image to grayscale

```bash
mogrify -type grayscale base.jpeg
```

## Convert an image into a c array

```bash
convert gray.jpeg -define h:format=rgba -depth 8 -size 953x668 grey_new.h
```
