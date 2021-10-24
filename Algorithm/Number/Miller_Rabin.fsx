open System
open System.Numerics

[<EntryPoint>]
let main argv =
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
            
        let bigint (x:int64) = bigint(x)
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
            let b = powMod (bigint n) a s


            true

        let testSet = [2; 325; 9375; 28178; 450775; 9780504; 1795265022] // [2; 7; 61] for 32-bit integers.

        testSet |> List.forall (millerRabinTest <| bigint n)
    0