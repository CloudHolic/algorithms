module String

open System.Text

let get_Lcs strA strB =
    let lenA = String.length strA
    let lenB = String.length strB
    let lcs = new StringBuilder()

    let cache : int[,] = Array2D.zeroCreate <|| (lenA + 1, lenB + 1)
    
    let rec getLcs (str: StringBuilder) x y =
        match (x, y) with
        | (a, b) when cache.[a, b] = 0 -> str.ToString()
        | (a, b) when cache.[a, b] = cache.[a, b - 1] -> getLcs str a (b - 1)
        | (a, b) when cache.[a, b] = cache.[a - 1, b] -> getLcs str (a - 1) b
        | (a, b) when cache.[a, b] - 1 = cache.[a - 1, b - 1] -> getLcs (str.Insert(0, strA.[a - 1])) (a - 1) (b - 1)
        | _ -> ""

    for i in 1 .. lenA do
        for j in 1 .. lenB do
            match (i, j) with
            | (a, b) when strA.[a - 1] = strB.[b - 1] -> cache.[a, b] <- cache.[a - 1, b - 1] + 1
            | _ -> cache.[i, j] <- max <|| (cache.[i - 1, j], cache.[i, j - 1])
    
    cache.[lenA, lenB] |> ignore    // LCS Length
    getLcs lcs lenA lenB            // Full LCS