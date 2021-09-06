import System.Environment
import Data.Char

table1 = ["|", "y", "t", "2", "Q", "G", "Y", "A", ";", "u", "_", "R", "C", "e", "D", "0", "H", "/", "c", ")", "=", "N", "W", "V", "o", "&", "6", "n", "P", "k", "9", "$", "~", "d", "O", "K", "a", "?", ":", "<", "w", "8", "1", "T", "!", "f", "3", "i", "p", "]", "B", "x", "z", "l", "@", "s", "J", "j", "M", "r", "X", "S", "%", "#", "5"]

table2 = ["_", "c", "!", "P", "8", "/", ")", "[", "+", "X", "I", ":", "}", "i", "C", "$", "]", "#", "h", "Z", "W", ";", "2", "{", "?", "p", "y", "S", "G", "4", ">", "Q", "=", "H", "e", "~", "^", "3", "r", "0", "v", "6", "m", "a", "x", "u", "b", "f", "n", "&", "1", "N", "<", "A", "E", "O", "T", "K", "k", "w", "z", "D", "%", "M", "R"]

table3 = [";", ">", "]", "x", "+", "|", "c", "G", "[", "b", "V", "n", "H", "M", "S", ")", "#", "K", "<", "?", "3", "5", "B", "/", "N", "!", "_", "2", "y", "(", "a", "j", "%", "1", "h", "m", "W", "A", "E", "f", "v", "9", "Z", "t", "C", "F", "}", "o", "0", "$", "7", "s", "J", ":", "z", "I", "P", "Q", "R", "D", "Y", "4", "=", "u", "q"]

table4 = ["g", "@", "E", "(", "y", "x", ">", "c", "j", "_", "/", "R", "e", "L", "V", "9", "G", "~", "Q", "p", "1", "q", "&", "6", "P", "|", "^", "K", "C", "N", "u", "n", "?", "H", "W", "v", "J", "#", "=", "!", "0", "B", "}", "m", "]", "X", "U", "T", "*", "S", "f", "i", "I", "+", "M", "4", "D", "Z", "8", "<", "[", ")", ":", "5", "t"]

revDecToBin 0 = []
revDecToBin n
    | n `mod` 2 == 1 = 1 : revDecToBin (n `div` 2)
    | n `mod` 2 == 0 = 0 : revDecToBin (n `div` 2)

pad8 l
    | length l `mod` 8 == 0 = l
    | otherwise = pad8 (l ++ [0])

revStrToBin "" = []
revStrToBin (x : xs) = (revStrToBin xs) ++ (pad8 (revDecToBin (ord x)))

strToBin s = reverse (revStrToBin s) -- 1st

split l n = ((take n l), (drop n l))

binToBlockInternal [] r = r
binToBlockInternal l r
    | l2 == [] = l1 : r
    | otherwise = binToBlockInternal l2 (l1 : r)
    where (l1, l2) = split l 24

binToBlock l = binToBlockInternal l []

blockToSmallInternal [] r = r
blockToSmallInternal l r
    | l2 == [] = l1 : r
    | otherwise = blockToSmallInternal l2 (l1 : r)
    where (l1, l2) = split l 6

blockToSmall l
    | length l == 16 = [1, 0, 0, 0, 0, 0, 0] : (blockToSmallInternal (reverse (0 : 0 : (reverse l))) [])
    | length l == 8 = [1, 0, 0, 0, 0, 0, 0] : [1, 0, 0, 0, 0, 0, 0] : (blockToSmallInternal (reverse (0 : 0 : 0 : 0 : (reverse l))) [])
    | otherwise = blockToSmallInternal l []

blockToSmallMap l = map blockToSmall l

smallToDecInternal [] n = n
smallToDecInternal (x : xs) n = smallToDecInternal xs (2 * n + x)

smallToDec l = smallToDecInternal l 0

smallToDecMap l = map smallToDec l

smallToDecMapMap l = map smallToDecMap l

decToTableInternal table n = table !! n

decToTable tab1 tab2 tab3 tab4 l = [decToTableInternal tab1 (l !! 0), decToTableInternal tab2 (l !! 1), decToTableInternal tab3 (l !! 2), decToTableInternal tab4 (l !! 3)]

-- Here is where I made a mistake
-- Intended code was as you expected,
-- `decToTableMap l = map (decToTable table1 table2 table3 table4) l`
decToTableMap l = map (decToTable table1 table1 table1 table1) l

strFold l = foldl (++) "" l

strFoldMap l = map strFold l

strFoldMapFold l = strFold (strFoldMap l)

enc s = reverse (strFoldMapFold (decToTableMap (smallToDecMapMap (blockToSmallMap (binToBlock (strToBin s))))))

main = do
    s <- getProgName
    print (enc s)

