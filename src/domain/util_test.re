open Jest;

open Expect;

describe("util functions", () => {
  describe("querystring parsing", () => {
    test("with keys and values", (_) => {
      let result = Util.QueryParam.get("theKey", "something=else&theKey=dog");
      expect(result) |> toEqual(Some("dog"));
    });
    test("an empty value should represent none", (_) => {
      let result = Util.QueryParam.get("theKey", "theKey=");
      expect(result) |> toEqual(None);
    });
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