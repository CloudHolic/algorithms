type ConvexHullTrick =
    val mutable private pointer: int
    val private lines: ResizeArray<Line>
    member internal _.Inf = 10000000000L

    new () = { pointer = 0; lines = ResizeArray<Line>() }

    member private _.Intersect line1 line2 = (line2.b - line1.b |> float) / (line1.a - line2.a |> float)

    member this.AddLine a b =
        let newLine = { a = a; b = b; x = double -this.Inf }

        if this.lines.Count = 0 then this.lines.Add newLine
        else
            let mutable flag = true
            while this.lines.Count > 0 && flag do
                let top = this.lines[this.lines.Count - 1]
                let x = this.Intersect top newLine
                if x <= top.x then this.lines.RemoveAt(this.lines.Count - 1)
                else flag <- false

            newLine.x <- this.Intersect this.lines[this.lines.Count - 1] newLine
            this.lines.Add(newLine)

            if this.pointer >= this.lines.Count then
                this.pointer <- this.lines.Count - 1

    member this.Query x =
        while this.pointer < this.lines.Count - 1 && this.lines[this.pointer + 1].x < float x do
            this.pointer <- this.pointer + 1
        this.lines[this.pointer].eval x

and Line = {
    a: int64
    b: int64 
    mutable x: float } with

    member this.eval x = this.a * x + this.b    