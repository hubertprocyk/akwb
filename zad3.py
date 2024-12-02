def gc_count(seq: str):
    return (seq.count("C") + seq.count("G")) / len(seq) * 100 
def aa_stats(seq: str):
    return dict(
        A=seq.count("A") / len(seq),
        T=seq.count("T") / len(seq),
        C=seq.count("C") / len(seq),
        G=seq.count("G") / len(seq)
    )
 
dna_seq = "ATCG"
print("1. ", gc_count(dna_seq), "\n")
for k, v in aa_stats(dna_seq).items():
    print(f"{k}: {v}")