module FastFourierTransform =
    open System
    open System.Numerics

    let maxSize = 262144
    let pi = Math.PI
    let tau = 2. * pi

    module internal Details =
        let isPowerOf2 n = (n &&& n - 1) = 0

        let ilog2 n =
            if n < 2 then failwith "n must be greater than 1"
            if not (isPowerOf2 n) then failwith "n must be a power of 2"
            let rec loop n c s =
                let t = 1 <<< c
                if t = n then c
                elif t > n then
                    loop n (c - s) (s >>> 1)
                else
                    loop n (c + s) (s >>> 1)
            loop n 16 8

        let root a inv =
            if not inv then Complex.FromPolarCoordinates(1., a)
            else Complex.FromPolarCoordinates(1., a) |> Complex.Conjugate

        let roots inv =
            let unfolder c =
                if c < 2 * maxSize then
                    let vs = Array.init (c / 2) (fun i -> root (tau * float i / float c) inv)
                    Some (vs, c*2)
                else
                    None

            Array.unfold unfolder 1

        let rec loop n2 ln s c f t inv =
            if c > 2 then
                let c2 = c >>> 1
                let struct (t, f) = loop n2 (ln - 1) (s <<< 1) c2 f t inv
                let roots = (roots inv).[ln]

                if s > 1 then
                    for j = 0 to c2 - 1 do
                        let w = roots.[j]
                        let off = s * j
                        let off2 = off <<< 1;
                        for i = 0 to s - 1 do
                            let e = Array.get f (i + off2 + 0)
                            let o = Array.get f (i + off2 + s)
                            let a = w * o
                            Array.set t (i + off + 0) (e + a)
                            Array.set t (i + off + n2) (e - a)
                else
                    for j = 0 to c2 - 1 do
                        let w = roots.[j]
                        let e = Array.get f (2 * j + 0)
                        let o = Array.get f (2 * j + s)
                        let a = w * o
                        Array.set t (j + 0)   (e + a)
                        Array.set t (j + n2)  (e - a)

                struct (f, t)

            elif c = 2 then
                for i = 0 to s - 1 do
                    let e = Array.get f (i + 0)
                    let o = Array.get f (i + s)
                    let a = o
                    Array.set t (i + 0)   (e + a)
                    Array.set t (i + n2)  (e - a)

                struct (f, t)

            else
                struct (t, f)

    open Details

    let fft (vs : Complex []) invert : Complex [] =
        let n = vs.Length
        let ln = ilog2 n

        let vs0 = Array.copy vs
        let vs1 = Array.zeroCreate n

        let struct (_, t) = Details.loop (n >>> 1) ln 1 n vs0 vs1 invert
        if invert then t |> Array.map (fun i -> i / Complex(float n, 0.)) else t

    let multiply arr1 arr2 =
        let size = Array.length arr1 + Array.length arr2 - 1
        let size = Math.Pow(2., (Math.Log (float size) / Math.Log 2.) |> Math.Ceiling) |> int

        let coeff1 = Array.init size (fun i -> if i < Array.length arr1 then Complex(float arr1.[i], 0.) else Complex(0., 0.))
        let coeff2 = Array.init size (fun i -> if i < Array.length arr2 then Complex(float arr2.[i], 0.) else Complex(0., 0.))

        (fft coeff1 false, fft coeff2 false)
        ||> Array.map2 (fun x y -> x * y)
        |> function
            | res -> fft res true |> Array.map (fun x -> Math.Floor (x.Real + 0.5) |> int)