open Jest;

open Expect;

describe("The expense management editor", () =>
  describe("when checking if all fields are valid", () => {
    describe("with all valid fields", () => {
      let fields =
        Js.Dict.fromList([
          ("Description", true),
          ("Vendor", true),
          ("ExpenseType", true),
          ("Date", true),
          ("SubTotals", true),
          ("Total", true),
        ]);
      test("it indicate that the expense is valid", () =>
        expect(ExpenseManagementEditor.isExpenseValid(fields))
        |> toEqual(true)
      );
    });
    describe("with all invalid fields", () => {
      let fields =
        Js.Dict.fromList([
          ("Description", false),
          ("Vendor", false),
          ("ExpenseType", false),
          ("Date", false),
          ("SubTotals", false),
          ("Total", false),
        ]);
      test("it indicate that the expense is invalid", () =>
        expect(ExpenseManagementEditor.isExpenseValid(fields))
        |> toEqual(false)
      );
    });
    describe("with some fields valid and invalid", () => {
      let fields =
        Js.Dict.fromList([
          ("Description", true),
          ("Vendor", false),
          ("ExpenseType", false),
          ("Date", false),
          ("SubTotals", true),
          ("Total", false),
        ]);
      test("it indicate that the expense is invalid", () =>
        expect(ExpenseManagementEditor.isExpenseValid(fields))
        |> toEqual(false)
      );
    });
    describe("with only one field invalid", () => {
      let fields =
        Js.Dict.fromList([
          ("Description", true),
          ("Vendor", true),
          ("ExpenseType", true),
          ("Date", true),
          ("SubTotals", true),
          ("Total", false),
        ]);
      test("it indicate that the expense is invalid", () =>
        expect(ExpenseManagementEditor.isExpenseValid(fields))
        |> toEqual(false)
      );
    });
  })
);