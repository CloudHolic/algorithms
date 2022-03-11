namespace FsCollection

open System

type PersistentSegmentTree<'T> =
    val private op: 'T -> 'T -> 'T
    val private init: 'T
    val private rootCount: int
    val private maxSize: int
    val private nodes: ResizeArray<Node<'T>>    // Array of all nodes
    val private roots: int array                // Array of root indexes
    val mutable private lastRoot: int

    new (_rootCount, _maxSize, _op, _init) as this =
        {
            op = _op
            init = _init
            rootCount = _rootCount
            maxSize = _maxSize
            roots = Array.create (_rootCount + 1) 0
            nodes = new ResizeArray<Node<'T>>(2)
            lastRoot = 0
        } then this.Init()

    member private this.Init() =
        let rec init k st en =
            if st <> en then
                let mid = (st + en) >>> 1

                this.nodes[k].Left <- this.nodes.Count
                Node<'T>.CreateNew this.init |> this.nodes.Add

                this.nodes[k].Right <- this.nodes.Count
                Node<'T>.CreateNew this.init |> this.nodes.Add

                (this.nodes[k].Left, st, mid) |||> init
                (this.nodes[k].Right, mid + 1, en) |||> init

        this.roots[0] <- 1
        this.nodes.AddRange([|Node<'T>.CreateNew this.init; Node<'T>.CreateNew this.init|])
        init 1 1 this.maxSize

    member this.Query root left right =
        let rec query k st en =
            if left > en || right < st then this.init
            else if left <= st && en <= right then this.nodes[k].Value
            else
                let mid = (st + en) >>> 1
                ((this.nodes[k].Left, st, mid) |||> query, (this.nodes[k].Right, mid + 1, en) |||> query) ||> this.op

        query this.roots[root] 1 this.maxSize

    member this.Update root index value =
        let AddRoot() =
            let prevRoot = this.roots[this.lastRoot]
            this.nodes.Add({ Value = this.nodes[prevRoot].Value; Left = this.nodes[prevRoot].Left; Right = this.nodes[prevRoot].Right })
            this.lastRoot <- this.lastRoot + 1
            this.roots[this.lastRoot] <- this.nodes.Count - 1

        let rec update k st en =
            if st <> en then
                let mid = (st + en) >>> 1
                if index <= mid then
                    let now = this.nodes[k].Left
                    let newValue = (this.nodes[now].Value, value) ||> this.op
                    this.nodes.Add({ Value = newValue; Left = this.nodes[now].Left; Right = this.nodes[now].Right })
                    this.nodes[k].Left <- this.nodes.Count - 1
                    (this.nodes[k].Left, st, mid) |||> update
                else
                    let now = this.nodes[k].Right
                    let newValue = (this.nodes[now].Value, value) ||> this.op
                    this.nodes.Add({ Value = newValue; Left = this.nodes[now].Left; Right = this.nodes[now].Right })
                    this.nodes[k].Right <- this.nodes.Count - 1
                    (this.nodes[k].Right, mid + 1, en) |||> update

        if root > this.lastRoot then
            for _ = 1 to root - this.lastRoot do
                AddRoot()

        this.nodes[this.roots[root]].Value <- (this.nodes[this.roots[root]].Value, value) ||> this.op
        update this.roots[root] 1 this.maxSize

    member this.Clear() =
        Array.Clear(this.roots, 0, this.rootCount + 1)
        this.nodes.Clear()
        this.lastRoot <- 0
        this.Init()

and Node<'T> = {
    mutable Value: 'T
    mutable Left: int
    mutable Right: int } with

    static member CreateNew value = { Value = value; Left = 0; Right = 0 }