open Jest;

open Expect;

describe("Tax utilities", () =>
  describe("converting tax calculation methods", () => {
    test("from valid total first to corresponding tax calculation", () =>
      expect("totalFirst|15" |> Tax.Calculation.toMethod)
      |> toEqual(Tax.TotalFirst(15))
    );
    test("from valid subtotal first to corresponding tax calculation", () =>
      expect("subTotalFirst|15" |> Tax.Calculation.toMethod)
      |> toEqual(Tax.SubTotalFirst(15))
    );
    test("from completely invalid string to exempt tax calculation", () =>
      expect("mickeyMouse" |> Tax.Calculation.toMethod)
      |> toEqual(Tax.Exempt)
    );
    test(
      "from subTotalFirst with no percentage string to exempt tax calculation",
      () =>
      expect("subTotalFirst|" |> Tax.Calculation.toMethod)
      |> toEqual(Tax.Exempt)
    );
    test(
      "from subtotalFirst incorrect casing string to corresponding tax calculation",
      () =>
      expect("SubtotalfIrst|20" |> Tax.Calculation.toMethod)
      |> toEqual(Tax.SubTotalFirst(20))
    );
    test("from empty string to exempt tax calculation", () =>
      expect("" |> Tax.Calculation.toMethod) |> toEqual(Tax.Exempt)
    );
  })
);