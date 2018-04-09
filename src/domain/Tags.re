let toList = (tagString: string) =>
  tagString
  |> Js.String.split(",")
  |> Array.to_list
  |> List.filter(s => s !== "")
  |> List.map(s => s |> Js.String.trim);

let toCSV = (tags: list(string)) =>
  tags |> Array.of_list |> Js.Array.joinWith(",");