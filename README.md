# SpellCastSolver


## Word List
SCOWL: http://wordlist.aspell.net/scowl-readme/

Generate SCOWL Word List: http://app.aspell.net/create
After some testing we determined that it uses the 70 (large) SCOWL Size
http://app.aspell.net/create?max_size=70&spelling=US&max_variant=0&diacritic=strip&download=wordlist&encoding=utf-8&format=inline
Removed all words containing capital letters and apostrophes 


## Optimization Strategies

Have the root of the DFS start at the 2x square
Lookahead to see if any words could possibly beat the current highest potential score
Priority queue insead of stack. Prioritize words on the 2x or words
Space remaining (cornered)

Priority Algorithm:
  high point values
  



