open System
open System.Numerics

// Returns true if n is prime number.
let millerRabin (n: int64) = 
    let logPow mul sq (n: bigint) (k: int64) =
        let infSeq = n |> Seq.unfold (fun state -> Some(state, sq state))
        let binaryList = k |> List.unfold (fun state ->
            match state with
            | 0L -> None
            | _ -> Some(state % 2L, state / 2L))
        let expArr = (List.length binaryList, infSeq) ||> Seq.take |> Array.ofSeq

        binaryList
        |> List.mapi (fun i x ->
            match x with
            | 1L -> expArr.[i]
            | _ -> 1I)
        |> List.fold (fun acc x -> mul acc x) 1I
        
    let mulMod (m: bigint) (a: bigint) (b: bigint) = (a * b) % m
    let squareMod (m: bigint) (n: bigint) = (n * n) % m
    let powMod m = logPow (mulMod m) (squareMod m)

    // Execute Miller-Rabin Test with n and a.
    let millerRabinTest (n: bigint) (a: int) =
        let find2s n =
            let rec inner s d =
                let isOdd = d &&& 1I |> int
                match isOdd with
                | 1 -> (s, d)
                | _ -> inner <|| (s + 1, d >>> 1)

            inner 0 n

        let (s, d) = (n - 1I) |> find2s
        let b = powMod n (bigint a) (int64 d)

        match (a, n) with
        | _ when b = 1I || b = (n - 1I) -> true
        | _ -> b
                |> Seq.unfold (fun x -> let fx = (squareMod n x) in Some(x, fx))
                |> Seq.take s
                |> Seq.skip 1
                |> Seq.tryPick (fun x -> if x = 1I then Some(false) elif x = n - 1I then Some(true) else None)
                |> Option.exists id
                
    match n with
    | _ when n < 2L -> false
    | _ when n = 2L -> true
    | _ when n = 3L -> true
    | _ when n % 2L = 0L -> false
    | _ -> 
        [2; 3; 5; 7; 11; 13; 17; 19; 23; 29; 31; 37]
        |> List.filter (fun x -> (int64 x) < n)
        |> List.forall (millerRabinTest <| bigint n)