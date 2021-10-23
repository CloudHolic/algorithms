module String

// Assume that strings contain only lower cases.
let get_DL strA strB =
    let min4 (a, b, c, d) = min a <| (min b <| min c d)

    let lenA = String.length strA
    let lenB = String.length strB

    let cache : int[,] = Array2D.zeroCreate <|| (lenA + 2, lenB + 2)
    let da_cache: int[] = Array.init 26 (fun _ -> 1)

    cache.[0, 0] <- lenA + lenB
    for i in 1 .. lenA + 1 do
        cache.[i, 0] <- cache.[0, 0]
        cache.[i, 1] <- i - 1

    for j in 1 .. lenB + 1 do
        cache.[0, j] <- cache.[0, 0]
        cache.[1, j] <- j - 1

    for i in 2 .. lenA + 1 do
        let mutable db = 1

        for j in 2 .. lenB + 1 do
            let k = da_cache.[int strB.[j - 2] - int 'a']
            let l = db
            let mutable cost = 0

            match (i, j) with
            | _ when strA.[i - 2] = strB.[j - 2] -> db <- j
            | _ -> cost <- 1

            cache.[i, j] <- min4 (cache.[i, j - 1] + 1, cache.[i - 1, j] + 1, cache.[i - 1, j - 1] + cost, cache.[k - 1, l - 1] + i + j - k - l - 1)

        da_cache.[int strA.[i - 2] - int 'a'] <- i

    cache.[lenA + 1, lenB + 1]  // Damerau_Levenshtein Distance