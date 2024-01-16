# 11210CS410000 Computer Architecture

黃婷婷計算機結構作業

## Update

更新：加入更多測資（2024/1/12）

BUG:
1. 移除regex_search，改用alnum (在工作站上判斷錯誤，原因不明)
2. (a == tag) -> ((a >> 1) == (tag >> 1)) LSB 是  NRU Bit ，須去除後再比較
3. grading_example.sh 在 Ubuntu 22.04上無法正確顯示答案，須在工作站上執行，原因不明
4. 此更新版本於工作站上執行 testcases_2全部通過
5. 總得分81.75，可與LSB比較取得更高得分
