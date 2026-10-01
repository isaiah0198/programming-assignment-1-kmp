## 1. What I Built

I implemented the Knuth–Morris–Pratt (KMP) string-search algorithm in C++17, together with a naive string-search baseline and an empirical benchmarking harness. Both search functions locate the first occurrence of a pattern within a larger text and return the starting index of the match. If the pattern does not occur, they return \`-1\`, while an empty pattern returns \`0\`, treating it as matching at the beginning of the text. The original KMP paper describes the same general problem as finding a pattern as a contiguous substring of a larger text and introduces an algorithm whose running time is proportional to the combined lengths of the pattern and text (Knuth et al., 1977).

The implementation is divided into three functions. \`buildLPS()\` constructs the longest proper prefix which is also a suffix table for the pattern. \`kmpSearch()\` uses this table while scanning the text, and \`naiveSearch()\` provides a simpler baseline that tries the pattern at each possible starting position. CLRS describes the naive approach as explicitly considering each possible shift and comparing corresponding characters until either a mismatch occurs or the complete pattern matches. In the worst case, this can require \\(O((n-m+1)m)\\) time because previously discovered information is discarded when moving to the next starting position (Cormen et al., 2022).

The LPS table is the key supporting structure in my KMP implementation. As an example, for the pattern:

\`\`\`text

Pattern: A A B A A A B

LPS: 0 1 0 1 2 2 3

\`\`\`

Each LPS value records the length of the longest proper prefix of the pattern prefix ending at that position that is also a suffix. KMP makes use of this value to skip the pattern index to the next valid prefix after a mismatch has been found where part of the pattern has matched.The original paper explains this idea using a precomputed shift table that determines how far the pattern can move after a mismatch without rechecking text that is already known (Knuth et al., 1977).

The naive implementation was included as the baseline because it solves the same problem using the most direct strategy and therefore provides a clear comparison against KMP’s reuse of previous matching information. This distinction is particularly important for the empirical study, since practical performance does not always follow worst-case complexity alone. Horspool similarly frames string searching as a practical performance problem and notes that simple search programs have \\(O(mn)\\) worst-case behaviour while more sophisticated algorithms can reduce this substantially (Horspool, 1980).

I tested both algorithms using CTest and \`assert\`-based test cases covering matches at the beginning, middle and end of the text, no-match cases, repeated characters, empty patterns and patterns longer than the text. Inputs such as text \`AAAB\` with pattern \`AAB\` were useful because they exercise overlapping partial matches and helped confirm that the starting index returned by both algorithms was correct.

For the empirical component, I created a separate benchmark executable. Random inputs are produced using \`std::mt19937\` with a fixed seed of \`42\` to make the generated data reproducible. Each search is timed 51 times using std::chrono::steady_clock, and the median runtime is recorded to reduce the effect of timing noise and outliers. A \`volatile\` sink stores the result of each search so that the compiler cannot remove the timed call as unused work. The project uses C++17, CMake and CTest for implementation and testing, while Python with pandas and matplotlib is used only to process the benchmark CSV output and generate plots.

---

## 2. Empirical Study

### 2.1 Research Question

The empirical study investigates whether KMP’s \\(O(n+m)\\) worst-case guarantee translates into faster practical runtime than naive string search, and how the result changes depending on the structure of the input. The comparison focuses on two workloads: random text generated from a 26-character alphabet and highly repetitive text designed to create long partial matches.

### 2.2 Hypotheses

Before running the benchmarks, I formed three hypotheses:

- H1: Both algorithms would scale approximately linearly with increasing text length on random text.

- H2: On random text using a 26-character alphabet, naive search would remain competitive with KMP because most attempted matches would fail after only a small number of character comparisons.

- H3: On repetitive text, KMP would outperform naive search because the LPS table allows it to reuse information from previous partial matches instead of restarting the comparison from the beginning.

These expectations were based on the theoretical difference between the algorithms. CLRS gives KMP a linear matching phase and notes that naive search can be efficient on randomly generated strings because mismatches often occur early (Cormen et al., 2022).

### 2.3 Experimental Setup

The benchmarks were run on a MacBook Air with an Apple M4 processor and 16 GB of memory. The project was compiled using Apple Clang 21.0.0 with C++17, and CMake 4.4.2 was used to configure and build the project. Final benchmark measurements were collected using a Release build so that compiler optimisation was enabled consistently across both implementations.

Random text was generated using std::mt19937 with a fixed seed of 42, allowing the same generated data to be reproduced between runs. The random-text experiment used a fixed pattern length of 20 and an alphabet of 26 uppercase characters. Text sizes ranged from 1,000 to 10,000,000 characters. The repetitive experiment also used a pattern length of 20, but the text consisted almost entirely of A characters followed by a final B. Text sizes ranged from 1,000 to 1,000,000 characters.

Each algorithm was timed 51 times for every input size using std::chrono::steady_clock, and the median runtime was recorded. The median was chosen because individual timing measurements can vary due to factors such as operating-system scheduling, cache state, and other background activity. Before timing each generated input, both algorithms were run once and their returned indices were compared. If the results differed, the benchmark terminated instead of recording potentially invalid performance data.

Preliminary testing also showed that compiler optimisation affected the absolute timings and could change small differences between the algorithms. For this reason, only Release-build results were used in the final analysis.

### 2.4 Experiment 1: Random Text

The first experiment measured the effect of increasing text length while keeping the pattern length fixed at 20 and using a 26-character alphabet.

Figure 1. Random-text runtime comparison

Insert runtime_random.png here.

Both algorithms showed approximately linear growth as the text became larger, supporting H1. For example, at 100,000 characters KMP took approximately 0.091 ms, while naive search took approximately 0.081 ms. At 1,000,000 characters, the times increased to approximately 0.701 ms for KMP and 0.633 ms for naive search. At 10,000,000 characters, KMP required approximately 7.06 ms, compared with 6.40 ms for naive search.

Across the full random-text experiment, naive search was consistently slightly faster than KMP. The difference was relatively small, with naive taking roughly 84–93% of KMP’s runtime depending on the input size. This supports H2. Although naive search has a worse worst-case bound, random text from a 26-character alphabet rarely produces long partial matches. Most candidate positions fail after only one or a few comparisons, so the naive algorithm does not often encounter its expensive worst-case behaviour.

CLRS makes the same theoretical observation: when pattern and text characters are randomly selected, the expected number of comparisons performed by naive string matching remains small, making the method quite efficient on random inputs (Cormen et al., 2022).

The results therefore demonstrate an important distinction between asymptotic worst-case complexity and measured performance. KMP avoids redundant comparisons by maintaining the LPS table, but it also performs additional indexing and control-flow operations. On random inputs where naive search already fails quickly, this extra bookkeeping can outweigh the benefit of KMP’s more sophisticated fallback mechanism.

### 2.5 Experiment 2: Repetitive Text

The second experiment used a deliberately repetitive workload. The text consisted of a long sequence of A characters followed by a B, and the pattern had the same structure with length 20.

Figure 2. Repetitive-text runtime comparison

Insert runtime_repetitive.png here.

Figure 3. KMP speedup over naive search

Insert speedup.png here.

The difference between the algorithms became much larger under this workload. At a text length of 1,000 characters, KMP took approximately 0.000917 ms, while naive search took approximately 0.005042 ms, giving KMP a speedup of about 5.50×. At 100,000 characters, KMP required approximately 0.0928 ms, while naive search required approximately 0.528 ms, a speedup of about 5.69×. At 1,000,000 characters, KMP took approximately 0.946 ms, compared with 5.404 ms for naive search, producing a speedup of approximately 5.71×.

These results strongly support H3. In the repetitive input, naive search repeatedly matches most of the pattern before encountering the final mismatch. It then moves to the next candidate starting position and performs many of the same comparisons again. This is exactly the kind of behaviour identified in the original KMP paper, which uses repetitive strings as an example of where the straightforward approach performs unnecessary repeated work (Knuth et al., 1977).

KMP avoids much of this repetition by using the precomputed prefix information to determine the next valid position in the pattern after a mismatch. The original paper describes this as precomputing shifts so that the algorithm can move the pattern appropriately without backing up through the text (Knuth et al., 1977).

The observed speedup of roughly 5.7× is smaller than a simple character-comparison count might suggest. In the repetitive case, naive search can make almost 20 comparisons at many starting positions, while KMP performs substantially fewer. However, comparison count alone does not determine runtime. The naive implementation contains a very small and predictable inner loop that an optimising compiler and modern processor can execute efficiently. KMP performs additional LPS lookups, index updates, and branching. These constant-factor costs reduce the practical speedup even though KMP performs much less redundant matching work.

The study has several limitations. All measurements were collected on a single machine using one compiler and one operating environment, so the absolute results may differ on other hardware. The benchmark also used synthetic data rather than natural-language or application-specific text.

Only two input structures were investigated: uniformly random text and highly repetitive text. These represent useful contrasting cases, but they do not cover the full range of real workloads. The pattern length was also fixed at 20, so the results do not show how either implementation behaves as pattern length changes.

Timing very small inputs is another source of uncertainty because the measured execution times approach the scale of timer and system noise. Preliminary measurements also showed a warm-up effect, where the first execution could be noticeably slower than later ones. Using 51 repetitions and reporting the median reduces the influence of these effects but does not remove them completely.

Finally, the fixed random seed improves reproducibility but means that only one random input sequence was sampled for each text size. Repeating the experiment across several independent seeds would provide stronger evidence that the random-text results are representative.

### 2.7 Future Work

Future work could extend the benchmark by varying pattern length while holding text size constant. This would help show how the preprocessing cost of KMP changes relative to naive search as patterns become longer.

A second extension would vary alphabet size. Smaller alphabets should produce more frequent partial matches, which may gradually shift the advantage from naive search toward KMP.

It would also be useful to record the number of character comparisons performed by each implementation in addition to wall-clock time. This would separate the algorithmic amount of work from compiler and processor effects and provide a clearer explanation of why the measured speedup on repetitive input is smaller than the ratio suggested by comparison counts alone.

---

## 3. What I Learned

The most considerable conceptual barrier I encountered was understanding when to increase the text index in KMP and when to remain on the same character. In an earlier version of kmpSearch(), I increased i after a mismatch even though part of the pattern had already matched. Following the control flow helped me understand that when j > 0, KMP should instead use j = lps[j - 1] and compare the same text character again rather than advancing i. This was the point where I realised that KMP is not merely about “jumping ahead”; its real strength is preserving information from previous comparisons instead of discarding already completed work.

I also learned that automated tests themselves need to be checked critically. During testing, I initially had an incorrect expected result for the text \`AAAB\` and pattern \`AAB\`. Tracing the indices manually showed that the correct starting position was index \`1\`. This was a useful reminder that a failing test does not always mean the implementation is wrong; the test case or expected value can also contain an error.

The empirical study changed my understanding of algorithmic complexity. Before benchmarking, I expected KMP to be clearly faster because its worst-case complexity is better than naive search. The random-text results showed that this assumption was too simplistic. On random text with a 26-character alphabet, the two algorithms performed very similarly, with naive search often slightly faster. Most attempted matches failed quickly, so naive search rarely reached its expensive worst-case behaviour. I also observed that compiler optimisation affected the comparison: Debug and Release builds produced noticeably different relative results. This showed me that theoretical complexity describes how an algorithm scales, but practical performance also depends on input structure, compiler optimisation and implementation details.

Benchmarking also introduced a different kind of debugging. At one point, the repetitive-text experiment produced 72 result rows when I expected only 8. The plots showed several measurements stacked at the same text sizes, which made the problem easier to identify. Since \\(72 = 9 \times 8\\), I realised that the repetitive benchmark loop had accidentally been placed inside the nine-iteration random-text loop. Fixing the misplaced closing brace removed the duplicate measurements. This was a useful example of diagnosing a control-flow error from the structure of the output rather than from a compiler error.

Finally, I saw how noisy timing measurements can be. One early run produced a much higher runtime than nearby measurements, and the first repetitive benchmark was also noticeably slower than later repeats. These observations reinforced why a single timing result is unreliable. Measuring each instance 51 times so that I could report the median stabilized the benchmarks and helped me understand the difference between empirical algorithm analysis and measuring execution time once.

---

## 4. AI Use Disclosure

### Tools Used

I used Claude throughout the project as supporting tools. They were used for planning the assignment, explaining some KMP concepts, providing implementation hints, helping structure the benchmark, debugging build issues. I also used several KMP video explanations and academic references to improve my understanding of the algorithm rather than relying only on AI explanations. They can be found in my notes.md file.

### How I Used AI

AI was used most heavily during the planning and debugging stages. It helped me choose KMP for Track A and structure the project into implementation, testing, benchmarking, analysis and reporting tasks that could be completed within the available time.

For the core implementation, I used AI mainly for planning, some loose implementation guidance but mostly debugging. I worked through `buildLPS()`, `kmpSearch()`, `naiveSearch()` and the tested incrementally using some hints and quick reference tool. This was particularly useful when I was trying to understand why KMP does not always advance the text index after a mismatch.

I accepted some scaffolding more directly for parts that were not the central algorithmic focus, including the initial benchmark structure and parts of the repetitive-input experiment. I still tested these components and adjusted them where necessary.

AI was also used during debugging. This included help with CMake configuration, Git commands, benchmark structure, the nested-loop bug that duplicated repetitive results, and understanding why Debug and Release builds produced different benchmark behaviour.

### Incorrect or Unhelpful Output 1

One of the most serious pieces of advice I received involved an accidental Git repository in my home directory. The AI suggested that `rm -rf ~/.git` could be used to remove it if it was unintended. Before running the command, I checked the repository and discovered that it contained real commit history from another project I am creating. If I had copied the command without checking, I could have permanently deleted useful Git history.

This made it clear that destructive terminal commands should never be accepted without verifying exactly what path they affect and what data is stored there.

### Incorrect or Unhelpful Output 2

Another issue occurred when AI-generated benchmark code used an inconsistent alphabet-size label for the repetitive-text experiment. One version labelled the alphabet size as `1`, even though the generated input contained both `A` and `B`, meaning the effective alphabet size was `2`.

The search data itself was still valid, but the metadata in the CSV would have been misleading if left unchanged. I corrected the value to `2`. This showed me that even when generated code executes correctly, the surrounding labels and experiment metadata still need to be reviewed carefully.

### Other AI Limitations I Encountered

The original benchmark plan also predicted that naive search would likely be competitive with or faster than KMP on random text. This appeared true in the Debug build, but Release optimisation changed the relative performance and made the difference much smaller. The final results showed that performance predictions from AI should be treated as hypotheses rather than conclusions; the measured data should determine the analysis.

### Reflection

The project showed me that AI is useful for explaining unfamiliar concepts, generating scaffolding, suggesting experiments and helping diagnose errors, but it is most reliable when its output can be checked against tests, documentation or observed behaviour. I would use AI in a similar way again for planning, some loose implementation guidance but mainly for debugging. I would verify destructive commands, benchmark assumptions, metadata and performance claims before relying on them.

---

## References

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to algorithms* (4th ed.). MIT Press.

Horspool, R. N. (1980). Practical fast searching in strings. *Software: Practice and Experience, 10*(6), 501–506. [https://doi.org/10.1002/spe.4380100608](https://doi.org/10.1002/spe.4380100608)

Knuth, D. E., Morris, J. H., Jr., & Pratt, V. R. (1977). Fast pattern matching in strings. *SIAM Journal on Computing, 6*(2), 323–350. [https://doi.org/10.1137/0206024](https://doi.org/10.1137/0206024)
