open Jest;

open Expect;

let buildOrder = name => {
  let order: OrderData.Order.newOrder = {customerName: name, orderItems: []};
  order;
};

describe("saving a new order", () =>
  test("it should add the new order to the store", () => {
    let testOrder = buildOrder("Rosinda");
    let result = CafeStore.add(testOrder) |> ignore;
    /* expect(result |> List.length) |> toBe(5); */
    expect(1) |> toBe(2);
  })
);