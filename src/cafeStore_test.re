open Jest;

open Expect;

module FakeCafeStore = CafeStore.Make(PouchdbFake);

let buildOrder = name => {
  let order: OrderData.Order.newOrder = {customerName: name, orderItems: []};
  order;
};

describe("saving a new order", () =>
  test("it should add the new order to the store", () => {
    let testOrder = buildOrder("Rosinda");
    let result = FakeCafeStore.add(testOrder) |> ignore;
    /* expect(result |> List.length) |> toBe(5); */
    expect(1) |> toBe(2);
  })
);