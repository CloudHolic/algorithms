namespace FsCollection

type Queue<'T> (frontList: 'T list, rBackList: 'T list) =
    let mutable hashCode = None
    member internal _.frontList = frontList
    member internal _.rBackList = rBackList

    member _.Peek =
        match frontList with
        | head :: _ -> head
        | _ -> raise(new System.Exception("Queue is empty"))

    member _.TryPeek =
        match frontList with
        | head :: _ -> Some head
        | _ -> None

    member _.Enqueue x =
        match frontList, x :: rBackList with
        | [], r -> Queue((List.rev r), [])
        | f, r -> Queue(f, r)

    member _.Dequeue =
        match frontList with
        | head :: tail -> 
            match tail, rBackList with
            | [], r -> head, Queue((List.rev r), [])
            | f, r -> head, Queue(f, r)
        | _ -> raise (new System.Exception("Queue is empty"))

    member _.TryDequeue =
        match frontList with
        | head :: tail ->
            match tail, rBackList with
            | [], r -> Some (head, Queue((List.rev r), []))
            | f, r -> Some (head, Queue(f, r))
        | _ -> None

    member _.IsEmpty = frontList.IsEmpty

    member _.Length = frontList.Length + rBackList.Length

    member _.Reverse() =
        match rBackList, frontList with
        | [], r -> Queue((List.rev r), [])
        | f, r -> Queue(f, r)

    override this.GetHashCode() =
        match hashCode with
        | None ->
            let mutable hash = 1
            for x in this do
                hash <- 31 * hash + Unchecked.hash x
            hashCode <- Some hash
            hash
        | Some hash -> hash

    override this.Equals(other) =
        match other with
        | :? Queue<'T> as y ->
            (this :> System.IEquatable<Queue<'T>>).Equals y
        | _ -> false

    interface System.IEquatable<Queue<'T>> with
        member this.Equals (y: 'T Queue) =
            if this.Length <> y.Length then false
            else
                if this.GetHashCode() <> y.GetHashCode() then false
                else Seq.forall2 (Unchecked.equals) this y

    interface System.Collections.Generic.IEnumerable<'T> with
        member _.GetEnumerator() : System.Collections.Generic.IEnumerator<'T> =
            let e = seq {
                yield! frontList
                yield! (List.rev rBackList)}
            e.GetEnumerator()

    interface System.Collections.IEnumerable with
        override this.GetEnumerator() =
            (this :> System.Collections.Generic.IEnumerable<'T>).GetEnumerator()
            :> System.Collections.IEnumerator

    interface System.Collections.Generic.IReadOnlyCollection<'T> with
        member this.Count = this.Length

[<RequireQualifiedAccess>]
module Queue =
    let empty<'T> : 'T Queue = Queue<_>([], [])

    let inline peek (q: 'T Queue) = q.Peek

    let inline tryPeek (q: 'T Queue) = q.TryPeek

    let inline enqueue (x: 'T) (q: 'T Queue) = q.Enqueue x

    let inline dequeue (q: 'T Queue) = q.Dequeue

    let inline tryDequeue (q: 'T Queue) = q.TryDequeue

    let (|Cons|Nil|) (q: 'T Queue) = match q.TryDequeue with Some(a, b) -> Cons(a, b) | None -> Nil

    let inline isEmpty (q: 'T Queue) = q.IsEmpty

    let inline length (q: 'T Queue) = q.Length

    let inline rev (q: 'T Queue) = q.Reverse()

    let ofList xs = Queue<'T>(xs, [])

    let ofSeq xs = Queue<'T>((List.ofSeq xs), [])

    let inline toSeq (q: 'T Queue) = q :> seq<'T>

    let fold (f: 'State -> 'T -> 'State) (state: 'State) (q: 'T Queue) =
        let s = List.fold f state q.frontList
        List.fold f s (List.rev q.rBackList)

    let foldBack (f: 'T -> 'State -> 'State) (q: 'T Queue) (state: 'State) =
        let s = List.foldBack f (List.rev q.rBackList) state
        List.foldBack f q.frontList s