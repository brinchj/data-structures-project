


{-
  Heap Data Type
-}
data Heap = Heap {
        mini     :: Int
      , size     :: Int
      , children :: [Heap]
} deriving (Eq, Show)

instance (Ord Heap) where
    a < b = (mini a) < (mini b)

instance (Num Heap) where
    a + b = merge a b


{-
  Heap Functions
  * Is_Empty
  * Merge
  * Insert
  * Extract-min
-}

-- Check if heap is empty
is_empty :: Heap -> Bool
is_empty h = (size h) == 0

-- Merge two non-empty heaps
merge' :: Heap -> Heap -> Heap
merge' hA hB =
    let (hA' , hB') =
            if hA < hB then (hA, hB) else (hB, hA) in
    let m  = mini hA'
        s  = (size hA') + (size hB')
        cl = hB':(children hA') in Heap m s cl

-- Merge two heaps
merge :: Heap -> Heap -> Heap
merge hA hB =
    if is_empty hA then hB else if is_empty hB then hA
                                else merge' hA hB

-- Merge a list/queue of heaps using multi-pass
merge_many :: [Heap] -> Heap
merge_many (h:[]) = h
merge_many (h0:h1:tl) =
    merge_many (tl ++ [h0 + h1])

-- Insert an element
insert :: Heap -> Int -> Heap
insert h e =
    merge h (Heap e 1 [])

-- Extract minimum
extract_min :: Heap -> (Heap, Int)
extract_min h =
    (merge_many (children h), mini h)


-- Tests
test_insert h m 0 = h
test_insert h m n =
    test_insert (insert h (m-n+1)) m $! (n-1)

test_extract_min h 0 = h
test_extract_min h n =
    let (h', m) = extract_min $! h in
    test_extract_min h' $! (n-1)

benchmark h m n =
    let h' = test_insert h m n in
    test_extract_min h' n


-- Main
heap0 = Heap 1 4 [ Heap 2 1 [] , Heap 3 1 [], Heap 4 1 []]
main = do
  putStr (show (benchmark heap0 100 100))
