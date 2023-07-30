def quote_lines(str)
    str.lines.map { |e| "| #{e}".chomp }.join "\n"
end

io = [
    [
        "123 putn",
        "123",
    ],
    [
        "-45 putn",
        "-45"
    ],
    [
        "65 putc",
        "A",
    ],
    [
        "3 [ 4 add ] ! putn",
        "7",
    ],
    [
    
        "3 ; 4\n 5 add putn",
        "8",
    ],
    [
        '  "Hello  ',
        "Hello",
    ],
    [
        '"Hello 44 putc 32 putc "World!',
        "Hello, World!",
    ],
    [
        "[ 1 add ] :inc 4 inc 5 inc 6 inc putn putn putn",
        "765",
    ],
    [
        "[ 1 ] :test [ 2 ] :test\ntest putn",
        "2",
    ],
    [
        "[ [ 3 ] :me ] :you [ 9 ] :me\nme putn you me putn",
        "93",
    ],
    [
        "010030 putn",
        "10030",
    ],
    [
        "3 4 give putn take putn",
        "34",
    ],
    [
        "5 9 and putn",
        "1",
    ],
    [
        "5 9 or putn",
        "13",
    ],
    [
        "156 91203 mul putn",
        "14227668",
    ],
    [
        "3 4 div putn",
        "0",
    ],
    [
        "21 4 div putn",
        "5",
    ],
    [
        "24 5 mod putn",
        "4",
    ],
    [
        "6 7 cmp putn 32 putc\n7 7 cmp putn 32 putc\n8 7 cmp putn",
        "-1 0 1",
    ],
    [
        "15 203\n1 0  2 roll putn putn",
        "15203",
    ],
    [
        "15 203\n0 1  2 roll putn putn",
        "20315",
    ],
    [
        "15 203 93\n2 1 0 3 roll putn putn putn",
        "1520393",
    ],
    [
        "15 203 93\n0 0 0 3 roll putn putn putn",
        "151515",
    ],
    [
        "9 10 11 12 0 -4 1 roll putn putn putn putn putn",
        "91211109",
    ],
    [
        "1 2 3 4 0 roll putn putn putn putn",
        "4321",
    ],
    [
        "3;4\n5 ; 6\nputn putn",
        "53",
    ],
    [
        '12 [ "truthy ] [ ] if',
        "truthy",
    ],
    [
        '0 [ "truthy ] [ ] if',
        "",
    ],
    [
        "7 0 3 1 5 1 [ putn ] [ ] while",
        "53",
    ],
    [
        "5 [ 0 0 1 1 3 roll putn -1 add ] [ 0 0 1 1 3 roll ] while",
        "54321",
    ],
    #  #std functions
    [
        "#std 5 7 swap putn putn",
        "57",
    ],
    [
        "#std 3 4 sub putn",
        "-1",
    ],
    [
        "#std 4 5 dup putn putn",
        "55",
    ],
    [
        "#std 1 2 3 pop putn putn",
        "21",
    ],
    [
        "#std 3 4 eq putn 4 4 eq putn 5 4 eq putn",
        "010",
    ],
    [
        "#std NL",
        "\n",
    ],
    [
        "#std SP",
        " ",
    ],
    [
        "#std 4 posq putn 0 posq putn -4 posq putn",
        "100",
    ],
    [
        "#std 9 9 9 9 3 down putn putn putn putn",
        "0123",
    ],
    [
        "#std 5 0 down putn putn",
        "05",
    ],
    [
        '#std 5 [ "is-one ] [ "not-one ] [ 1 eq ] ifel',
        "not-one",
    ],
    [
        '#std 1 [ "is-one ] [ "not-one ] [ 1 eq ] ifel',
        "is-one",
    ],
    [
        "#std 4 5 6 7 8\n3 nrev putn putn putn putn putn",
        "67854",
    ],
    [
        "#std 33 72 97 104 97 46 46 46\n7 puts",
        "Haha...",
    ],
    [
        "#std 65 66 67 0 puts putn",
        "67",
    ],
]

executable = ARGV[0] || "mkgg.exe"

failed = 0
total = io.size
io.each.with_index(1) { |(input, expected), i|
    print "#{i}/#{total}\r"
    result = IO.popen(executable, "r+") { |pipe|
        # we need io to complete tasks anyhow
        pipe.write "%io\n"
        pipe.write input
        pipe.close_write
        pipe.read
    }
    if expected != result
        puts "Test case ##{i} (of #{total}) failed. Input:"
        puts quote_lines input
        puts "Expected:"
        puts quote_lines expected
        puts "Got:"
        puts quote_lines result
        failed += 1
    end
}
if failed.zero?
    puts "All #{total} test cases passed."
else
    puts "Passed #{total - failed}/#{total} cases."
end
