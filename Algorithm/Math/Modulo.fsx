module Modulo =
    let AddMod m x y = (x + y) % m
    let MulMod m x y = (x * y) % m
    let SquareMod m n = (n * n) % m

    let InverseMod m n =
        let rec inner n i g e l a =
            match e with
            | 0L -> g
            | _ -> let o = n / e
                   inner e l a (n - o * e) (i - o * l) (g - o * a)
        (m + (inner m 1L 0L n 0L 1L)) % m

    let logPow mul sq (n: int64) (k: int64) =
        let infSeq = n |> Seq.unfold (fun state -> Some(state, sq state))
        let binaryList = k |> List.unfold (fun state ->
            match state with
            | 0L -> None
            | _ -> Some(state % 2L, state / 2L))
        let expArr = (List.length binaryList, infSeq) ||> Seq.take |> Array.ofSeq

        binaryList
        |> List.mapi (fun i x ->
            match x with
            | 1L -> expArr[i]
            | _ -> 1L)
        |> List.fold (fun acc x -> mul acc x) 1L
      
    let PowMod m = logPow (MulMod m) (SquareMod m)

let modulo = 1000000007L

let Add = Modulo.AddMod modulo
let Sub x y = modulo - y |> Add x
let Mul = Modulo.MulMod modulo
let Inverse = Modulo.InverseMod modulo
let Pow = Modulo.PowMod modulo