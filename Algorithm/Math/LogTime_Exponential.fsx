module Number

let exp n k =
    let infSeq = n |> Seq.unfold (fun state -> Some(state, state * state))
    let binaryList = k |> List.unfold (fun state ->
        match state with
        | 0 -> None
        | _ -> Some(state % 2, state / 2))
    let expArr = (List.length binaryList, infSeq) ||> Seq.take |> Array.ofSeq

    binaryList
    |> List.mapi (fun i x ->
            match x with
            | 1 -> expArr.[i]
            | _ -> 1)
    |> List.fold (fun acc x -> acc * x) 1