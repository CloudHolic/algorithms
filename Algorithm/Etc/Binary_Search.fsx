let binSearch arr target =
    let rec inner arr pos =
        match Array.length arr with
        | 0 -> -1
        | i -> 
            let middle = i / 2
            match sign <| compare target arr[middle] with
            | 0  -> middle + pos
            | -1 -> inner arr[.. middle - 1] pos
            | _  -> inner arr[middle + 1 ..] (pos + middle + 1)
    
    inner arr 0

let lowerBound (arr:'T array) target =
    let rec inner beg en = 
        match sign <| compare beg en with
        | -1 -> 
            let mid = (beg + en) >>> 1
            match sign <| compare arr[mid] target with
            | -1  -> inner (mid + 1) en
            | _ -> inner beg mid
        | _ -> beg

    inner 0 (Array.length arr)

let upperBound (arr:'T array) target =
    let rec inner beg en =
        match sign <| compare beg en with
        | -1 ->
            let mid = (beg + en) >>> 1
            match sign <| compare arr[mid] target with
            | 1 -> inner beg mid
            | _ -> inner (mid + 1) en
        | _ -> beg

    inner 0 (Array.length arr)