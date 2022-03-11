open System
open System.Numerics

module FastFourierTransform =
    let tau = 2. * Math.PI

    let root a inv =
        if not inv then Complex.FromPolarCoordinates(1., a)
        else Complex.FromPolarCoordinates(1., a) |> Complex.Conjugate

    let fft (vs : Complex []) invert : Complex [] =
        let n = vs.Length

        let mutable result = Array.copy vs

        let rec loop ln inv =
            if ln <= n then
                let roots = root (tau / float ln) inv

                for i in 0 .. ln .. n - 1 do
                    let mutable w = Complex(1., 0.)
                    for j = 0 to ln / 2 - 1 do
                        let u = result[i + j]
                        let v = result[i + j + ln / 2] * w

                        result[i + j] <- u + v
                        result[i + j + ln / 2] <- u - v

                        w <- w * roots

                loop (ln <<< 1) inv
        
        let mutable j = 0
        for i = 1 to n - 1 do
            let mutable bit = n >>> 1
            while j >= bit do
                j <- j - bit
                bit <- bit >>> 1
            j <- j + bit

            if i < j then
                let temp = result[i]
                result[i] <- result[j]
                result[j] <- temp

        loop 2 invert
        if invert then result |> Array.map (fun i -> i / Complex(float n, 0.)) else result

    let multiply arr1 arr2 =
        let LeastSquare k =
            if k = 0 then 0
            else
                let rec loop i t =
                    if i >= 64 then t
                    else loop (i * 2) (t ||| (t >>> i))
                loop 1 (k - 1) + 1

        let size = Array.length arr1 + Array.length arr2 - 1 |> LeastSquare

        let coeff1 = Array.init size (fun i -> if i < Array.length arr1 then Complex(float arr1[i], 0.) else Complex(0., 0.))
        let coeff2 = Array.init size (fun i -> if i < Array.length arr2 then Complex(float arr2[i], 0.) else Complex(0., 0.))

        (fft coeff1 false, fft coeff2 false)
        ||> Array.map2 (fun x y -> x * y)
        |> function
            | res -> fft res true |> Array.map (fun x -> Math.Floor (x.Real + 0.5) |> int)