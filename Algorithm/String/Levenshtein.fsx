module String

let get_Levenshtein strA strB =
    let min3 a b c = min a <| min b c
    let boolToInt = 
        function
        | true -> 1
        | false -> 0

    let lenA = String.length strA
    let lenB = String.length strB
    
    let cache : int[,] = Array2D.zeroCreate <|| (lenA + 1, lenB + 1)

    for i in 0 .. lenA do
        for j in 0 .. lenB do
            match (i, j) with
            | (0, 0) -> cache.[i, j] <- 0
            | (a, 0) -> cache.[i, j] <- a
            | (0, b) -> cache.[i, j] <- b
            | _ -> cache.[i, j] <- min3 <||| (cache.[i - 1, j] + 1, cache.[i, j - 1] + 1, cache.[i - 1, j - 1] + (boolToInt <| (strA.[i - 1] <> strB.[j - 1])))

    cache.[lenA, lenB]  // Levenshtein Distance