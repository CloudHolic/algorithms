open System
open System.Numerics

// Very naive and slow implementation of FFT.
module FastFourierTransform =
    let tau = 2. * Math.PI

    let root a inv = 
        if not inv then Complex.FromPolarCoordinates(1., a)
        else Complex.FromPolarCoordinates(1., a) |> Complex.Conjugate

    let fft x invert =
        let rec inner x invert =
            match x, invert with
            | [||], _ -> [||]
            | [|x|], _ -> [|x|]
            | x, invert ->
                x
                |> Array.mapi (fun i c -> i % 2 = 0, c)
                |> Array.partition fst
                |> fun (even, odd) -> inner (Array.map snd even) invert, inner (Array.map snd odd) invert
                ||> Array.mapi2 (fun i even odd ->
                    let btf = odd * (root (tau * (float i / float x.Length)) invert)
                    even + btf, even - btf)
                |> Array.unzip
                ||> Array.append

        inner x invert
        |> function
            | list ->
                if invert then list |> Array.map (fun i -> i / Complex(float x.Length, 0.))
                else list

    let multiply arr1 arr2 =
        let size = Array.length arr1 + Array.length arr2 - 1
        let size = Math.Pow(2., (Math.Log (float size) / Math.Log 2.) |> Math.Ceiling) |> int

        let coeff1 = Array.init size (fun i -> if i < Array.length arr1 then Complex(float arr1.[i], 0.) else Complex(0., 0.))
        let coeff2 = Array.init size (fun i -> if i < Array.length arr2 then Complex(float arr2.[i], 0.) else Complex(0., 0.))

        (fft coeff1 false, fft coeff2 false)
        ||> Array.map2 (fun x y -> x * y)
        |> function
            | res -> fft res true |> Array.map (fun x -> Math.Floor (x.Real + 0.5) |> int)