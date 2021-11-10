namespace FsCollection

type IPriorityQueue<'T when 'T : comparison> =
    abstract member Peek : unit -> 'T
    abstract member TryPeek : unit -> 'T option

    abstract member Insert : 'T -> IPriorityQueue<'T>
    
    abstract member Pop : unit -> 'T * IPriorityQueue<'T>
    abstract member TryPop : unit -> ('T * IPriorityQueue<'T>) option
        
    abstract member IsEmpty : bool with get
        
    abstract member Length : int
    
type Heap<'T when 'T : comparison> (isDescending: bool, length: int, data: HeapData<'T>) =
    member internal _.heapData = data
    member internal _.heapLength = length

    member _.Head = 
        match data with
        | E -> raise (new System.Exception("Heap is empty"))
        | T(x, _) -> x

    member _.TryHead =
        match data with
        | E -> None
        | T(x, _) -> Some x

    member _.Insert x = Heap.merge isDescending (length + 1) (T(x, [])) data

    member _.Tail() = 
        let mergeData (h1: HeapData<'T>) (h2: HeapData<'T>) : HeapData<'T> =
            match h1, h2 with
            | E, h -> h
            | h, E -> h
            | T(x, xs), T(y, ys) ->
                if isDescending then
                    if x <= y then T(y, h1::ys) else T(x, h2::xs)
                else
                    if x <= y then T(x, h2::xs) else T(y, h1::ys)

        match data with
        | E -> raise (new System.Exception("Heap is empty"))
        | T(_, xs) ->
            let combine state item =
                match state with
                | Some p, l -> (None, (mergeData item p)::l)
                | None, l -> (Some item, l)

            let tail =
                xs
                |> List.fold combine (None, [])
                |> function
                    | Some i, l -> i::l
                    | None, l -> l
                |> List.fold mergeData E
            Heap(isDescending, (length - 1), tail)

    member this.Pop() =
        match data with
        | E -> raise (new System.Exception("Heap is empty"))
        | T(x, _) -> x, this.Tail()

    member this.TryPop() =
        match data with
        | E -> None
        | T(x, _) -> Some(x, this.Tail())

    member _.IsEmpty =
        match data with
        | E -> true
        | _ -> false
            
    member _.Length = length

    member _.IsDescending = isDescending

    static member private merge isDescending newLength (h1 : HeapData<'T>) (h2 : HeapData<'T>) : Heap<'T> =
        match h1, h2 with
        | E, h -> Heap(isDescending, newLength, h)
        | h, E -> Heap(isDescending, newLength, h)
        | T(x, xs), T(y, ys) ->
            if isDescending then
                if x <= y then Heap(isDescending, newLength, T(y, h1::ys)) else Heap(isDescending, newLength, T(x, h2::xs))
            else
                if x <= y then Heap(isDescending, newLength, T(x, h2::xs)) else Heap(isDescending, newLength, T(y, h1::ys))

    interface IPriorityQueue<'T> with        
        member this.Peek() = this.Head
        member this.TryPeek() = this.TryHead

        member this.Insert element = this.Insert element :> IPriorityQueue<'T>

        member this.Pop() = 
            let element, newHeap = this.Pop()
            element, (newHeap :> IPriorityQueue<'T>)
        member this.TryPop() =
            match this.TryPop() with
            | Some(element, newHeap) -> Some(element, newHeap :> IPriorityQueue<'T>)
            | None -> None

        member this.IsEmpty = this.IsEmpty
        member this.Length = this.Length

and HeapData<'T when 'T : comparison> =
    | E
    | T of 'T * list<HeapData<'T>>
    
[<RequireQualifiedAccess>]
module PriorityQueue =
    let empty<'T when 'T : comparison> isDescending = Heap<'T>(isDescending, 0, E) :> IPriorityQueue<'T>

    let inline peek (pq: IPriorityQueue<'T>) = pq.Peek()

    let inline tryPeek (pq: IPriorityQueue<'T>) = pq.TryPeek()

    let inline push (pq: IPriorityQueue<'T>) x = pq.Insert x

    let inline pop (pq: IPriorityQueue<'T>) = pq.Pop()

    let inline tryPop (pq: IPriorityQueue<'T>) = pq.TryPop()

    let inline isEmpty (pq: IPriorityQueue<'T>) = pq.IsEmpty

    let inline length (pq: IPriorityQueue<'T>) = pq.Length