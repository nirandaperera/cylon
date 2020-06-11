import os
from os.path import expanduser

home = expanduser("~")
# join_exec = f"{home}/git/twisterx/build/bin/table_api_test_hash"
join_exec = f"../../../../build/bin/table_api_test_hash"
print(f"twx home: {join_exec}")

csv1 = f"/tmp/csv1.csv"
csv2 = f"/tmp/csv2.csv"

out_dir = f"/tmp/twx_join_test/"
print(f"output dir: {out_dir}")
os.system(f"rm -rf {out_dir}; mkdir -p {out_dir}")

cols = 4
key_duplication_ratio = 0.1  # on avg there will be rows/key_range_ratio num of duplicate keys

# for i in [1000000]:
for i in [10000, 100000, 1000000, 10000000, 100000000]:
    test_dir = f"{out_dir}/{i}"
    os.system(f"rm -rf {test_dir}; mkdir -p {test_dir}")

    os.system(f"python ./generate_csv.py -o {csv1} -r {i} -c {cols} --krange 0 {int(i * key_duplication_ratio)}")
    os.system(f"python ./generate_csv.py -o {csv2} -r {i} -c {cols} --krange 0 {int(i * key_duplication_ratio)}")

    os.system(f"{join_exec}")

    os.system(f"mv {csv1} {test_dir}")
    os.system(f"mv {csv2} {test_dir}")
    for j in ['right', 'left', 'inner', 'outer']:
        os.system(f"mv {home}/h_out_{j}.csv {test_dir}/")
        os.system(f"mv {home}/s_out_{j}.csv {test_dir}/")

    print(f"##### test {i} done!")