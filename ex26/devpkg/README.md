The Simple Package Management Tool {#mainpage}
==============================================

### NAME
devpkg -- Simple package management utility

### SYNOPSIS

~~~
devpkg -I url | -L | -c configure-options | -m make-options | -i install-options | -d path | -S | -F url -B url
~~~

### DESCRIPTION
The **devpkg** helps to *fetch*, *configure*, *build* and *install* packages in forms of `.tar.gz`, `.tar.bz2` and `.git` from internet.

> You **MUST** use `devpkg -S` to initalize the data base first.

### FUNCTION LETTERS
`-I url`\n
	Install package from `url`, it will fetch, configure, make, install then clean up the fetch source code and other build-phase temporatily files

`-L`\n
   List urls of the installed packages if any

`-c configure-options`\n
   Setup the ./configure opruons

`-m make-options`\n
   Setup the make options

`-i install-options`\n
   Setup install options

`-d path`\n
   Install all urls in the 'path' file, which contains url per line to install\n
   If You fetch depents file from internet, the file MUST be `*.DEPENDS`
    
`-S`\n
   Initalize the data base to store package urls which are installed

`-F url`\n
   Fetch the package source code from `url`

`-B url`\n
   Configure and Build the package of `url`

### ENVIRONMENT
The `devpkg -S` will creates a database file at `/usr/local/.devpkg/db`, which will record the installed package's urls.\n
When `fetch`, `build` or `install` package, the package compressed source code is at /tmp/pkg-src.tar.[gz|bz2], and the build-phase temporatily files is at /tmp/pkg-build. After install the package, those two directorues will be cleanup.

### EXAMPLE
Fetch package source from url: `foo.bar`.
~~~
devpkg -F foo.bar
~~~

Install package from url: `foo.bar` using configure options:`c-opts`, make options: `m-opts`, install options: `i-opts`.
~~~
devpkg -c c-opts -m m-opts -i i-opts -I foo.bar
~~~

List package we have installed if any.
~~~
devpkg -L
~~~

### BUGS

### TODO
- [ ] Some packages need ./autoconfig, but we don't implement it.
- [ ] `devpkg -d xxx.DEPENDS` can't input configure options, make options, and install options.
- [ ] Don't check the integrity(ex. md5).
