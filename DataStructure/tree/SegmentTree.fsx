namespace FsCollection

type SegmentTree<'T> = class
    val private op: 'T -> 'T -> 'T
    val private init: 'T
    val private node: 'T array
    val private size: int
    val private leafCount: int

    new (count: int, _op, _init) =
        let lc = SegmentTree<'T>.LeastSquare count + 1 in
        {
            op = _op
            init = _init
            leafCount = lc
            size = count
            node = Array.create (2 * lc) _init
        }

    static member private LeastSquare k =
        if k = 0 then 0
        else
            let rec loop i t =
                if i >= 64 then t
                else loop (i * 2) (t ||| (t >>> i))
            loop 1 (k - 1) + 1

    static member private LeftChild k = k * 2
    static member private RightChild k = k * 2 + 1

    member this.Query left right =
        let rec query k st en =
            if left > en || right < st then this.init
            else if left <= st && en <= right then this.node.[k]
            else
                let mid = (st + en) >>> 1
                ((SegmentTree<'T>.LeftChild k, st, mid) |||> query, (SegmentTree<'T>.RightChild k, mid + 1, en) |||> query) ||> this.op

        query 1 1 this.size

    member this.Update index value =
        let rec update k st en =
            if index < st || index > en then this.node.[k]
            else if st = en then
                this.node.[k] <- value
                this.node.[k]
            else
                let mid = (st + en) >>> 1
                this.node.[k] <- ((SegmentTree<'T>.LeftChild k, st, mid) |||> update, (SegmentTree<'T>.RightChild k, mid + 1, en) |||> update) ||> this.op
                this.node.[k]

        update 1 1 this.size |> ignore
end