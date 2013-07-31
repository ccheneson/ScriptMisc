ThumbGene
=============

This C program has been developped in 2001.
Its purpose was to generate thumbnails from original pictures for the application phpwebgallery 
It uses the gd library to generate the newly resized pictures.

User guide:
- Install the gd library. On Ubuntu system,
`apt-get install libgd2-noxpm-dev libgd2-noxpm`

- Typing `./ThumbGene --help` will present you the availables options:

    Usage: ./ThumbGene [options] 
     -h      --help             Display this usage information.
     -p      --phpwebgallery    Create thumbnails for phpwebgallery application.
     -f      --factor           Enter scale factor.By default, it's 4.
     -s      --size             Enter the size of x and y. Choices are 1024768, 640480 or 160120
     -v      --verbose          Print verbose messages.

