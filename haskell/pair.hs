import Data.Monoid
import System.Random
import qualified Data.Sequence as S
import qualified List


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

instance (Monoid Heap) where
    mempty  = Heap (-1) 0 []
    mappend = merge


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

-- Generic merger - strategy lies in case of empty input list
merger_create :: ([Heap] -> Heap) -> [Heap] -> [Heap] -> Heap
merger_create f [] []  = mempty
merger_create f [] [h] = h
merger_create f [h] [] = h
merger_create f []  q  = f q
merger_create f [h] q  = f (h:q)
merger_create f (h0:h1:hs) q =
    merger_create f hs $! ((mappend h0 h1):q)

-- Merge a list/queue of heaps using multiple (n) passes
merge_npass hs =
    let f = \q -> merger_create f (List.reverse q) [] in
    merger_create f hs []

-- Merge a list/queue of heaps using two passes
merge_2pass hs =
    let f = \q -> mconcat q in
    merger_create f hs []

-- Switch between 2pass/npass here
merger = merge_npass


-- Insert an element
insert :: Heap -> Int -> Heap
insert h e =
    merge h (Heap e 1 [])

-- Extract minimum
extract_min :: Heap -> (Heap, Int)
extract_min h =
    if is_empty h then (h, -1) else
        (merger (children h), mini h)


-- Tests
randomList :: (Random a) => Int -> Int -> [a]
randomList seed m = take m (randoms (mkStdGen seed))

test_insert :: Heap -> [Int] -> Heap
test_insert h []     = h
test_insert h (e:tl) =
    test_insert (insert h e) $! tl

test_extract_min h 0 = h
test_extract_min h n =
    let (h', m) = extract_min $! h in
    test_extract_min h' $! (n-1)

benchmark h m =
    let h' = test_insert h $! (randomList 42 m) in
    test_extract_min h' m


-- Main
heap0 = Heap 91 4 [ Heap 2 1 [] , Heap 3 1 [], Heap 4 1 []]
main = do
  putStr (show (benchmark heap0 20000))
