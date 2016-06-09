# perl -w ralf.pl DIRNAME
# RALF: Replace All Line Feeds (with the running platform's default line ending)
$txtdir  = $ARGV[0] || ".";
chdir($txtdir);
opendir(DIR, ".");
@txtFiles = grep( -T, readdir(DIR));
closedir(DIR);
$td = 'ralf'; # $0;
mkdir $td;

foreach $file (@txtFiles) {
    open(FILE, $file) ||
        do { print "Can't open input file $file: $!\n"; next; };
    open(FOUT, '>', "$td/$file") ||
        do { print "Can't open output file $file: $!\n"; next;};
    while (<FILE>) {
        s/\015/\n/g;
        chomp;
        print FOUT $_, "\n";
    }
    print FOUT "\n";
    close(FILE);
    close(FOUT);
}

