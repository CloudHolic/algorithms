namespace FsCollection

type Stack<'T> (internalList: 'T list) =
    let mutable hashCode = None
    member internal _.internalList = internalList

    member _.Peek =
        match internalList with
        | head :: _ -> head
        | _ -> raise (new System.Exception("Stack is empty"))

    member _.TryPeek =
        match internalList with
        | head :: _ -> Some head
        | _ -> None

    member _.Push x = Stack(x :: internalList)

    member _.Pop =
        match internalList with
        | head :: tail -> head, Stack(tail)
        | _ -> raise (new System.Exception("Stack is empty"))

    member _.TryPop =
        match internalList with
        | head :: tail -> Some (head, Stack(tail))
        | _ -> None
    
    member _.IsEmpty = internalList.IsEmpty

    member _.Length = internalList.Length

    member _.Reverse() = Stack(List.rev internalList)

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
        | :? Stack<'T> as y ->
            (this :> System.IEquatable<Stack<'T>>).Equals y
        | _ -> false

    interface System.IEquatable<Stack<'T>> with
        member this.Equals (y: 'T Stack) =
            if this.Length <> y.Length then false
            else
                if this.GetHashCode() <> y.GetHashCode() then false
                else Seq.forall2 Unchecked.equals this y

    interface System.Collections.Generic.IEnumerable<'T> with
        member _.GetEnumerator() : System.Collections.Generic.IEnumerator<'T> =
            let e = Seq.ofList internalList
            e.GetEnumerator()

    interface System.Collections.IEnumerable with
        override this.GetEnumerator() =
            (this :> System.Collections.Generic.IEnumerable<'T>).GetEnumerator()
            :> System.Collections.IEnumerator

    interface System.Collections.Generic.IReadOnlyCollection<'T> with
        member this.Count = this.Length

[<RequireQualifiedAccess>]
module Stack =
    let empty<'T> : 'T Stack = Stack<_>([])

    let inline peek (s: 'T Stack) = s.Peek

    let inline tryPeek (s: 'T Stack) = s.TryPeek

    let inline push (x: 'T) (s: 'T Stack) = s.Push x

    let inline pop (s: 'T Stack) = s.Pop

    let inline tryPop (s: 'T Stack) = s.TryPop

    let (|Cons|Nil|) (s: 'T Stack) = match s.TryPop with Some(a, b) -> Cons(a, b) | None -> Nil
    
    let inline isEmpty (s: 'T Stack) = s.IsEmpty
    
    let inline length (s: 'T Stack) = s.Length

    let inline rev (s: 'T Stack) = s.Reverse()

    let ofList xs = Stack<'T>(xs)

    let ofSeq xs = Stack<'T>(Seq.toList xs)

    let inline toSeq (s: 'T Stack) = s :> seq<'T>

    let fold (f: 'State -> 'T -> 'State) (state: 'State) (s: 'T Stack) =
        List.fold f state s.internalList

    let foldBack (f: 'T -> 'State -> 'State) (s: 'T Stack) (state: 'State) =
        List.foldBack f s.internalList state