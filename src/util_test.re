open Jest;

open Expect;

describe("util functions", () => {
  test("querystring parsing", (_) => {
    let result = Util.QueryParam.get("theKey", "something=else&theKey=dog");
    expect(result) |> toEqual(Some("dog"));
  });
  describe("react string to element shortcut", () => {
    test("react string with some", () => {
      let result = Util.sopt(Some("hello1"));
      expect(result) |> toEqual(ReasonReact.stringToElement("hello1"));
    });
    test("react string with normal string", () => {
      let result = Util.s("hello2");
      expect(result) |> toEqual(ReasonReact.stringToElement("hello2"));
    });
  });
});