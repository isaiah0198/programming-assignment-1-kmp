13:25  First test of the project. LPS for AABAAAB = 0 1 0 1 2 2 3. The 5 index indicates when a fallback occurs; the length decreases from 2 to lps[1] = 1, and a match occurs again. 

13:40 The kmpSearch function has passed its first tests. The correct return value is i - j since i is one past the end of the match, so the start is i - j. The primary complication came with understanding when the text index i should advance and when to keep it at the same letter. In my first code version of kmpSearch, I advanced i even after a mismatch with j being larger than 0. It was a mistake I discovered while trying to understand the essence of the algorithm rather than because of facing a problem. The reason behind this issue is that KMP must use j = lps[j - 1] to determine a shorter valid prefix against the same text letter. Advancing i can skip some character that must be checked and miss a valid match. At first, I wanted to modify the LPS array during searching, which helped me realize that the LPS table should be built in advance and treated as read-only thereafter. 

14:02 naiveSearch also works after correcting some errors in loops and comparison logic. I can now see that i selects every possible start in the text and j checks the pattern from that starting point. In case of any mismatch, next i begins the new round of comparisons. What caused some problems in naiveSearch was understanding the function of each index and translating the algorithm into concrete loop conditions. I initially used n - m by itself as the loop condition instead of checking whether the candidate start index satisfied i <= n - m. Also, I wrote text[i + j] && pattern[j], which is correct in C++ but does not check equality. I also made a mistake when checking whether a match had occurred by comparing i and j with if (i == j) instead of checking whether the pattern index had reached the full pattern length with if (j == m). The main lesson is that it is important to understand the meaning of every index: in naive search, i takes values of candidates for the text position while j progresses through the pattern and in KMP i moves through the text and j moves back due to lps.

15:56 Added AAAB/AAB test. First wrote expected = 4, the assert failed. The code was right, the test was wrong (a 3-char pattern can't start at index 4 in a 4-char string). Tests can be wrong too.

10:58 Added randomText and repetitiveText. Verified that randomText(20, 4) produced a 20-character string containing only A–D, and that the output was identical across repeated program runs because of the fixed seed. repetitiveText(10, 'A', 'B') correctly produced AAAAAAAAAB. Benchmark target compiled and ran successfully. Next I will be writing out the median timing helper.

Tested the benchmark timing helper on a 10,000-character random string with pattern ABCDEFG. Both KMP and naive search returned -1, confirming they agreed on the result. The median timings over 11 runs were approximately 203,042 ns for KMP and 167,416 ns for naive search. This was an early indication that KMP is not necessarily faster on random inputs, despite its better worst-case complexity. I Re-ran the timing test and observed different absolute runtimes while both algorithms still returned the same result. KMP measured approximately 253,708 ns and naive search 226,625 ns. This confirmed that individual benchmark timings vary between executions, reinforcing the decision to use repeated measurements and report the median rather than a single run.

Added a correctness check to the benchmark harness before collecting timing data. For random text sizes from 1,000 up to 10,000,000 characters, KMP and naive search returned the same result for every generated input. No disagreement was detected, so the benchmark can now move on to timing the two implementations. Experiment 1 successfully measured KMP and naive search over random text ranging from 1,000 to 10,000,000 characters while keeping the pattern length fixed at 20 and the alphabet size fixed at 26. Both implementations showed approximately linear growth at larger input sizes. Naive search was generally slightly faster than KMP on this workload, which suggests that KMP's better worst-case complexity does not automatically make it faster on random text. With a 26-character alphabet, mismatches often occur near the beginning of the pattern, so naive search avoids much of its potential repeated work. One run produced an unusually high naive-search result at 10,000 characters, showing that execution-time measurements can contain noise even when repeated measurements and the median are used. This was the first benchmark result that challenged my initial intuition. I expected KMP to simply be faster because it has better asymptotic worst-case complexity, but the random-input results showed why empirical testing matters.

Completed Experiment 1 and saved the results to results/text_size.csv. The benchmark measured both KMP and naive search across random text sizes from 1,000 to 10,000,000 characters, using a fixed pattern length of 20 and alphabet size of 26. The output was verified as valid CSV, and the full automated test suite still passed afterward. The results showed approximately linear growth for both algorithms, with naive search generally slightly faster on random high-alphabet input.

Although the repetitive input suggests a much larger theoretical reduction in character comparisons, the measured speedup was smaller because runtime depends on more than comparison count. The naive implementation has a very small, predictable inner loop that modern compilers and CPUs can execute efficiently, while KMP performs additional LPS lookups and more complex branching. This reduces the practical gap between the algorithms even when KMP performs substantially less redundant comparison work.

Release optimisation materially changed the random-input results. In the unoptimised build, naive search was consistently faster by a small margin, while the optimised build made the two algorithms roughly equal and allowed KMP to become slightly faster at larger input sizes. This showed that small benchmark differences can be dominated by compiler settings rather than algorithmic complexity alone.

The first repetitive benchmark was noticeably slower than subsequent measurements, which suggests a warm-up effect from factors such as cold caches and CPU frequency scaling. This reinforced the need to use repeated measurements and a median rather than relying on the first timing result.

## KMP Learning Resources Watched

I used several video explanations while learning KMP and the LPS/failure-function logic. These were used as supplementary learning resources rather than as implementation sources.

1. [Abdul Bari: KMP String Matching Algorithm](https://www.youtube.com/watch?v=V5-7GzOfADQ)
   - Used as an introductory whiteboard walkthrough of KMP.
   - Helped reinforce the idea of avoiding repeated comparisons after a mismatch.

2. [Knuth-Morris-Pratt (KMP) algorithm](https://www.youtube.com/watch?v=4jY57Ehc14Y)
   - Visual explanation of KMP search and LPS construction.
   - Particularly useful for connecting the LPS table to the code structure I implemented.

3. [KMP Algorithm Visually Explained](https://www.youtube.com/watch?v=ynv7bbcSLKE)
   - Used for another visual explanation of how the pattern shifts after mismatches.

4. [KMP FULLY Explained (With Animation)](https://www.youtube.com/watch?v=kPB9ibKhWNU)
   - Helped reinforce the movement of the text and pattern indices.

5. [KMP Explained: Linear Time String Matching Made Simple](https://www.youtube.com/watch?v=vsKshRcboNA)
   - Used as a general explanation of why KMP achieves linear-time matching.

6. [KMP Visually Explained (with Python code)](https://www.youtube.com/watch?v=q4_90fOoS-s)
   - helpful when comparing the algorithmic logic across another implementation language.

7. [KMP tutorial with failure function in Java](https://www.youtube.com/watch?v=EL4ZbRF587g)
   - Used to understand the relationship between the failure function and prefix fallback.

8. [KMP Pattern Matching Algorithm, step by step](https://www.youtube.com/watch?v=xWdAgFregt0)
   - Additional step-by-step walkthrough.

9. [KMP with multiple dry runs](https://www.youtube.com/watch?v=qases-9gOpk)
    - Longer deep-dive resource with multiple dry runs.
    - Not essential to the implementation, but useful as a reference for deeper tracing.