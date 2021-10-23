module Number

let primeList n m =
    let increment x = x + 1
    let table = Array.create (m + 1) true
    let limit = m |> float |> sqrt |> int |> increment
    let seqs i = Seq.init (m / i / 2) (fun e -> i * (e * 2 + 3))
    let rec check factor cur =
        match cur with
        | c when c > Array.length table - 1 -> ()
        | _ -> seqs factor |> Seq.iter (fun x -> if x <= m then table.[x] <- false)

    table.[1] <- false
    table |> Array.iteri (fun i _ -> if i > 2 && i % 2 = 0 then table.[i] <- false)
    [3..2..limit] |> List.iter (fun x ->
        match table.[x] with
        | true -> check x <| x + x
        | _ -> ())
        
    let mutable result = []
    table |> Array.iteri (fun i x ->
        match (i, x) with
        | (i, _) when i < n -> ()
        | (i, x) when x -> result <- [i] |> List.append result
        | _ -> ())
    result