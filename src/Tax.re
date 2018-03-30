type taxCalculationMethod =
  | Exempt
  | TotalFirst(int)
  | SubTotalFirst(int);

module Calculation = {
  let toDelimitedString = (meth: taxCalculationMethod) =>
    switch (meth) {
    | TotalFirst(rate) => "totalFirst|" ++ string_of_int(rate)
    | SubTotalFirst(rate) => "subTotalFirst|" ++ string_of_int(rate)
    | Exempt => "exempt|"
    };
  let toMethod = (str: string) =>
    switch (
      str
      |> Js.String.split("|")
      |> Array.to_list
      |> List.filter(s => s !== "")
      |> List.map(s => s |> Js.String.toLocaleLowerCase)
    ) {
    | ["totalfirst", rate] => TotalFirst(int_of_string(rate))
    | ["subtotalfirst", rate] => SubTotalFirst(int_of_string(rate))
    | _ => Exempt
    };
};